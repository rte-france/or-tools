package com.google.ortools.java;

import com.google.ortools.Loader;
import com.google.ortools.linearsolver.*;
import java.lang.RuntimeException;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class XpressCallbackTest {
    // Numerical tolerance for checking primal, dual, objective values
    // and other values.
    private static final double NUM_TOLERANCE = 1e-5;

    static class CustomException extends RuntimeException {
        public CustomException() {
        }

        public CustomException(String msg) {
            super(msg);
        }

        public CustomException(Throwable throwable) {
            super(throwable);
        }

        public CustomException(String message, Throwable cause) {
            super(message, cause);
        }
    }

    @BeforeEach
    public void setUp() {
        Loader.loadNativeLibraries();
    }

    static class MyMpCallback extends MPCallback {
        private final MPSolver mpSolver;
        private final boolean throwRuntimeException;
        private int nSolutions = 0;
        private List<Double> lastVarValues = null;

        public MyMpCallback(MPSolver mpSolver, boolean throwRuntimeException) {
            super(false, false);
            this.mpSolver = mpSolver;
            this.throwRuntimeException = throwRuntimeException;
        }

        @Override
        public void runCallback(MPCallbackContext callback_context) {
            if (throwRuntimeException) {
                throw new CustomException("this is a test exception in a callback");
            }
            if (!callback_context.event().equals(MPCallbackEvent.MIP_SOLUTION)) {
                return;
            }
            nSolutions++;
            if (!callback_context.canQueryVariableValues()) {
                return;
            }
            lastVarValues = new ArrayList<>();
            for (MPVariable variable : mpSolver.variables()) {
                lastVarValues.add(callback_context.variableValue(variable));
            }
        }
    }

    private MPSolver initSolver() {
        MPSolver solver = MPSolver.createSolver("XPRESS_MIXED_INTEGER_PROGRAMMING");

        int nVars = 30;
        int maxTime = 30;
        solver.objective().setMaximization();

        Random rand = new Random(123);
        for (int i = 0; i < nVars; i++) {
            MPVariable x = solver.makeIntVar(-rand.nextDouble() * 200, rand.nextDouble() * 200, "x_" + i);
            solver.objective().setCoefficient(x, rand.nextDouble() * 200 - 100);
            if (i==0) {
                continue;
            }
            double rand1 = -rand.nextDouble() * 2000;
            double rand2 = rand.nextDouble() * 2000;
            MPConstraint constraint = solver.makeConstraint(Math.min(rand1, rand2), Math.max(rand1, rand2));
            constraint.setCoefficient(x, rand.nextDouble() * 200 - 100);
            for (int  j = 0; j < i ; j++) {
                constraint.setCoefficient(solver.variable(j), rand.nextDouble() * 200 - 100);
            }
        }

        solver.setSolverSpecificParametersAsString("PRESOLVE 0 MAXTIME " + maxTime);
        solver.enableOutput();
        return solver;
    }

    @Test
    public void testXpressNewMipSolCallback() {
        MPSolver solver = initSolver();
        if (solver == null) {
            return;
        }

        MyMpCallback cb = new MyMpCallback(solver, false);
        solver.setCallback(cb);

        solver.solve();


        // This is a tough MIP, in 30 seconds XPRESS should have found at least 5
        // solutions (tested with XPRESS v9.0, may change in later versions)
        assertTrue(cb.nSolutions > 5);
         // Test that the last solution intercepted by callback is the same as the optimal one retained
        for (int i = 0; i < solver.numVariables(); i++) {
            assertEquals(solver.variable(i).solutionValue(), cb.lastVarValues.get(i), NUM_TOLERANCE);
        }
    }

    @Test
    public void testCallbackThrowsException() {
        // Test that when the callback throws an exception, it is caught by or-tools
        MPSolver solver = initSolver();
        if (solver == null) {
            return;
        }

        MyMpCallback cb = new MyMpCallback(solver, true);
        solver.setCallback(cb);

        assertDoesNotThrow(() -> solver.solve());
    }
}
